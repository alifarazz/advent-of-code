(def input-text (slurp "src/advent/2/input.txt"))

(def input-vec (clojure.string/split input-text #"\n"))

;; part A
(defn checksum [freqs num]
  (count
   (filter #(not (empty? %))
        (map (fn [freq]
               (map #(first %1)
                    (filter #(= (second %) num)
                            freq)))
             freqs))))

(let [freqs (map frequencies input-vec)]
  (* (checksum freqs 2) (checksum freqs 3))) ; part A

;; part B
(defn solve-partB []
  (defn diff-str
    ([seq1 seq2] (diff-str seq1 seq2 0))
    ([seq1 seq2 num-diff]
     (if (empty? seq1)
       num-diff
       (recur (rest seq1)
              (rest seq2)
              (+ num-diff (if (= (first seq1) (first seq2)) 0 1))))))
  (defn common-id  [elem-idx]
    (let [x (.indexOf
             (map #(diff-str (first %) (second %))
                  (nth
                   (map
                    (fn [x]
                      (map #(list x %) input-vec))
                    input-vec) elem-idx)) 1)]
      (if(= x -1)
        (recur (inc elem-idx))
        (nth input-vec x))))
  (defn find-diff [seq1 seq2 res]
    (cond (empty? seq1) res
          (= (first seq1) (first seq2))  (find-diff (rest seq1)
                                                    (rest seq2)
                                                    (str res (first seq1)))
          :else (find-diff (rest seq1)
                           (rest seq2)
                           res)))
  (def the2str
    (let [first-str (common-id 0)]
      (list
       (nth input-vec
            (.indexOf
             (map #(diff-str first-str %) input-vec)
             1))
      first-str)))
  (find-diff (first the2str) (second the2str) "")) ;

(solve-partB)                           ; part B
